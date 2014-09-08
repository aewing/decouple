<?hh // strict
require_once dirname(__FILE__) . "/../Fixtures/HttpRouterFixture.hh";
use HackPack\HackUnit\Core\TestCase;
use Decouple\Http\Request\Request;
use Decouple\Http\Request\Uri;
use Decouple\Decoupler\Decoupler;
use Decouple\Http\Router\Router;
class HttpRouterTest extends TestCase {

  public function testRouteFunctionA() : void {
    $uri = new Uri('/foo/bar');
    $result = $this->_testRoute($uri);
    $this->expect($result)->toEqual('bar42');
  }

  public function testRouteFunctionB() : void {
    $uri = new Uri('/bar/foo');
    $result = $this->_testRoute($uri);
    $this->expect($result)->toEqual('42foo');
  }

  public function testRouteMethodA() : void {
    $uri = new Uri('/baz/bar');
    $result = $this->_testRoute($uri);
    $this->expect($result)->toEqual('Controller::/baz/bar/42');
  }

  public function testRouteMethodB() : void {
    $uri = new Uri('/baz/bang');
    $result = $this->_testRoute($uri);
    $this->expect($result)->toEqual('Controller::bang/42');
  }

  private function _testRoute(Uri $uri) : mixed {
    $request = new Request($uri, Map::fromArray(['baz'=>true]), Map {}, Map {});
    $dependencies = Map {
      "RouteDependency" => new RouteDependency(),
      "Decouple\Http\Request\Uri" => $uri,
      "Decouple\Http\Request\Request" => $request,
      "RoutePropertyDependency" => new RoutePropertyDependency(),
      "RouteConstructorDependency" => new RouteConstructorDependency()
    };
    $decoupler = new Decoupler($dependencies);
    $router = new Router($decoupler);
    $router->serve('/foo/([^/]+)', function(Request $request, RouteDependency $dep) : mixed {
      return $request->routeParams->at(1) . $dep->double(21);
    });
    $router->serve('/bar/([^/]+)', function(Request $request, RouteDependency $dep) : mixed {
      return $dep->double(21) . $request->routeParams->at(1);
    });
    $router->serve('/baz/bar', ['TestController','baz']);
    $tc = $decoupler->injectInstance('TestController');
    $router->serve('/baz/([^/]+)', [$tc,'bang']);
    $function = $router->route($request);
    $result = $decoupler->inject($function);
    return $result;
  }
}
