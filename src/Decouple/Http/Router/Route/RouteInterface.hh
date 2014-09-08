<?hh // strict
namespace Decouple\Http\Router\Route;
use Decouple\Decoupler\Decoupler;
use Decouple\Http\Request\Request;
interface RouteInterface {
  public function execute(Request $request) : mixed;
}
