<?hh // strict
namespace Decouple\Http\Router;
use Decouple\Http\Router\Route\AbstractRoute;
use Decouple\Decoupler\Decoupler;
use Decouple\Http\Request\Request;
class Router {
  private Vector<AbstractRoute> $routes;
  public function __construct(protected Decoupler $decoupler) {
    $this->routes = Vector {};
  }
  public function serve(string $pattern, mixed $function=null) : int {
    if(is_callable($function)) {
      if(is_array($function)) {
        $route = new \Decouple\Http\Router\Route\MethodRoute($pattern, $function);
      } else {
        $route = new \Decouple\Http\Router\Route\FunctionRoute($pattern, $function);
      }
      return $this->add($route);
    }
    throw new \Exception(sprintf("Invalid route function provided for route %s", $pattern));
  }
  public function add(AbstractRoute $route) : int {
    $this->routes->add($route);
    return $this->routes->count() - 1 ?: 0;
  }
  public function get(int $id) : AbstractRoute {
    return $this->routes->at($id);
  }
  public function route(Request $request) : mixed {
    foreach($this->routes as $route) {
      $matches = $route->matches($request->uri);
      if(count($matches)) {
        $request->routeParams = $matches;
        return $route->execute($request);
      }
    }
    throw new \Exception(sprintf("Unable to find a route for url [%s]", $request->uri));
  }
}