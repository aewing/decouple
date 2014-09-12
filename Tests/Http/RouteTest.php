<?hh // partial 
require_once dirname(__FILE__) . "/../Fixtures/HttpRouterFixture.php";
class HttpRouteTest extends PHPUnit_Framework_TestCase {
  public function testRoute() : void {
    $dependencies = Map {
      "RouteDependency" => new RouteDependency() 
    };

    $uri = new Decouple\Http\Request\Uri('foo/bar');
    $decoupler = new Decouple\Decoupler\Decoupler($dependencies);
    $request = new Decouple\Http\Request\Request($uri, Map::fromArray(['baz'=>true]), Map {}, Map {});
    $route = new Decouple\Http\Router\Route\FunctionRoute('foo/([^/]+)');
    $route->from(function(RouteDependency $dep) : mixed {
      return $dep->double(21);
    });
    $function = $route->execute($request);
    $result = $decoupler->injectFunction($function);
    $this->assertEquals($result, 42);
  }
}
