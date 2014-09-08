<?hh // strict
namespace Decouple\Http\Router\Route;
use Decouple\Http\Request\Request;
use Decouple\Http\Request\Uri;
abstract class AbstractRoute implements RouteInterface {

  public function __construct(private string $pattern, protected mixed $callback=null) {
    // Do nothing
  }
  
  abstract public function execute(Request $request) : mixed;

  public function matches(Uri $uri) : Vector<string> {
    $matches = [];
    preg_match_all('|' . $this->pattern . '|', $uri, $matches);
    $result = Vector {};
    if(is_array($matches)) {
      $result->addAll($this->reduce($matches));
    }
    return $result;
  }

  private function reduce(KeyedTraversable<mixed,string> $arr) : Vector<string> {
    $res = Vector {};
    array_walk_recursive($arr, function($a) use ($res) { $res[] = $a; });
    return $res;
  }

  public function from(mixed $source) : AbstractRoute {
    return $this;
  }
}
