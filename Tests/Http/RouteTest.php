<?hh // partial 
require_once dirname(__FILE__) . "/../Fixtures/HTTPRouterFixture.php";
class HTTPRouteTest extends PHPUnit_Framework_TestCase {
  public function testRoute() : void {
    $dependencies = Map {
      "RouteDependency" => new RouteDependency() 
    };

    $uri = new Decouple\HTTP\Request\Uri('foo/bar');
    $decoupler = new Decouple\Decoupler\Decoupler($dependencies);
    $request = new Decouple\HTTP\Request\Request($uri, Map::fromArray(['baz'=>true]), Map {}, Map {});
    $route = new Decouple\HTTP\Router\Route\FunctionRoute('foo/([^/]+)');
    $route->from(function(RouteDependency $dep) : mixed {
      return $dep->double(21);
    });
    $function = $route->execute($request);
    $result = $decoupler->injectFunction($function);
    $this->assertEquals($result, 42);
  }
}
