<?hh // partial 
require_once dirname(__FILE__) . "/../Fixtures/HTTPRouterFixture.php";
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Request\Uri;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Router\Router;
class HTTPRouterTest extends PHPUnit_Framework_TestCase {

  public function testRouteFunctionA() : void {
    $uri = new Uri('/foo/bar');
    $result = $this->_testRoute($uri);
    $this->assertEquals($result, 'bar42');
  }

  public function testRouteFunctionB() : void {
    $uri = new Uri('/bar/foo');
    $result = $this->_testRoute($uri);
    $this->assertEquals($result, '42foo');
  }

  public function testRouteMethodA() : void {
    $uri = new Uri('/baz/bar');
    $result = $this->_testRoute($uri);
    $this->assertEquals($result, 'TestController::baz:42');
  }

  public function testRouteMethodB() : void {
    $uri = new Uri('/baz/bang');
    $result = $this->_testRoute($uri);
    $this->assertEquals($result, 'Controller::bang/42');
  }

  private function _testRoute(Uri $uri) : mixed {
    $request = new Request($uri, Map::fromArray(['baz'=>true]), Map {}, Map {});
    $dependencies = Map {
      "RouteDependency" => new RouteDependency(),
      "Decouple\HTTP\Request\Uri" => $uri,
      "Decouple\HTTP\Request\Request" => $request,
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
