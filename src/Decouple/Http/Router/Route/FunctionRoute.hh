<?hh // strict
namespace Decouple\Http\Router\Route;
class FunctionRoute extends AbstractRoute implements RouteInterface {
  public function from(mixed $function) : FunctionRoute {
    $this->callback = $function;
    return $this;
  }
  public function execute(\Decouple\Http\Request\Request $request) : mixed {
    return $this->callback;
  }
}
