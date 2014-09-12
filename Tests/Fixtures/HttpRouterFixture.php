<?hh // strict
use Decouple\Http\Request\Request;
class RouteDependency {
  public function double(int $num) : int {
    return $num * 2;
  }
  public function half(int $num) : int {
    return (int)($num / 2);
  }
}

class RoutePropertyDependency {
  public function number() : int {
    return 21;
  }
}

class RouteConstructorDependency {
  public function number() : int {
    return 4;
  }
}
class TestController {
  public ?RoutePropertyDependency $pdep;
  public function __construct(private RouteConstructorDependency $odep) {}

  public function baz(Request $request, RouteDependency $dep) : string {
    return 'Controller::' . $request->routeParams->at(0) . '/' . $dep->double(21);
  }

  public function bang(Request $request, RouteDependency $dep) : string {
    $number = 0;
    if(!is_null($this->pdep)) {
      $number = $this->pdep->number();
    }
    return 'Controller::' . $request->routeParams->at(1) . '/' . $dep->half($number * $this->odep->number());
  }
}
