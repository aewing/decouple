<?hh // strict
class RouteDependency {
  public function double(int $number) : int {
    return $number * 2;
  }
}

use Decouple\HTTP\Request\Request;

class RoutePropertyDependency {}
class RouteConstructorDependency {}

class TestController {
  public function baz(Request $request, RouteDependency $dep) : string {
    return 'TestController::baz:' . $dep->double(21);
  }
  public function bang(Request $request, RouteDependency $dep) : string {
    return sprintf("Controller::%s/%s", $request->routeParams[1], $dep->double(21));
  }
}
