<?hh // strict
use Decouple\HTTP\Request\Request;
class TestHTTPService {}
class FooController {
  public function bar() : string {
    return "FooBar!";
  }
}
class BarController {
  public function bar(Request $request) : string {
    return $request->routeParams[0];
  }
  public function baz(Request $request) : string {
    return "baz";
  }
}
