<?hh // strict
require_once dirname(__FILE__) . "/../Fixtures/HttpRouterFixture.hh";
use HackPack\HackUnit\Core\TestCase;
class HttpRouteTest extends TestCase {
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
    $this->expect($result)->toEqual(42);
  }
}
