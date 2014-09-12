<?hh // strict
use Decouple\Http\Request\Request;
class TestService {}
class FooController {
  public function bar() : string {
    return "FooBar!";
  }
}
class BarController {
  public function baz(Request $request) : string {
    return $request->routeParams[1];
  }
}
