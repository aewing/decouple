<?hh // partial
require_once dirname(__FILE__) . '/../Fixtures/ConsoleAppFixture.php';
use Decouple\Console\App;
use Decouple\Console\Request\Request;
use Decouple\Decoupler\Decoupler;
class Console_AppTest extends PHPUnit_Framework_TestCase {
  public function testConsoleBootstrapA() : void {
    $app = $this->__bootstrap("decouple:version");
    $result = $this->capture($app);
    $this->assertEquals($result, "Decouple v0.1a\n");
  }

  public function testConsoleBootstrapB() : void {
    $app = $this->__bootstrap("foo:bar");
    $result = $this->capture($app);
    $this->assertEquals($result, "FooBar!\n");
  }

  public function testConsoleBootstrapC() : void {
    $app = $this->__bootstrap("bar:baz --bar=wtf!");
    $result = $this->capture($app);
    $this->assertEquals($result, "bar:wtf!\n");
  }

  public function __bootstrap(string $args) : App {
    $args = new Vector(explode(' ', $args));
    $request = new Request($args);
    $services = Map {"Decouple\Console\Request\Request" => $request};
    $decoupler = new Decoupler($services);
    $app = new App($request, $decoupler, Vector {
      "FooCommand",
      "BarCommand"
    });
    return $app;
  }

  public function capture(App $app) {
    ob_start();
    try {
      $app->execute();
    } catch(Exception $e) {
      echo $e->getMessage();
    }
    return ob_get_clean();
  }
}
