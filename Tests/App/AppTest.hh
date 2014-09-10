<?hh // strict
use HackPack\HackUnit\Core\TestCase;
use Decouple\Decoupler\Decoupler;
use Decouple\Http\Request\Request;
use Decouple\Http\Request\Uri;
use Decouple\Ui\Ui;
use Decouple\Http\Router\Router;
use Decouple\Http\Http;
use Decouple\Registry\Paths;
use Decouple\Log\Log;
require_once dirname(__FILE__) . '/../Fixtures/HttpAppFixture.hh';
class AppTest extends TestCase {
  public function testAppBootstrapA() : void {
    $app = $this->__bootstrap("/foo/bar");
    $result = $app->execute();
    $this->expect($result)->toEqual('FooBar!');
  }
  public function testAppBootstrapB() : void {
    $app = $this->__bootstrap("/bar/baz");
    $result = $app->execute();
    $this->expect($result)->toEqual('baz');
  }
  public function testAppBootstrapC() : void {
    $app = $this->__bootstrap("/bang/bam");
    $result = $app->execute();
    $this->expect($result)->toEqual('Bang!');
  }
  public function __bootstrap(string $uri) : Http {
    // Http request
    $uri = new Uri($uri);
    $request = new Request($uri, Map {}, Map {}, Map {});

    // Paths configuration
    $root = (string)realpath((string)dirname(__FILE__) . '/../../');
    $paths = new Paths(Map {
      "root" => $root,
      "app" => $root . '/app',
      "app.logs" => $root . '/app/logs',
      "ui" => $root . '/ui'
    });

    // Decoupler configuration
    $decoupler = new Decoupler(Map {
      "Decouple\Http\Request\Request" => $request,
      "Decouple\Http\Request\Uri" => $uri,
      "Decouple\Registry\Paths" => $paths,
      "Decouple\Ui\Ui" => new Decouple\Ui\Ui(),
    });

    // Router
    $router = new Router($decoupler);

    // Http application
    $app = new Http($request, $router, $decoupler);

    // Load application services from a map
    $services = Map {
      "Decouple\Log\Log" => new Log((string)$paths->get('app.logs') . "/http.log"),
      "TestService" => true
    };
    $app->addServices($services);

    // Load a single application service that must be instantiated
    $app->addService('Decouple\Ui\Ui');

    // Load a map of routes
    $routes = Map {
        "/foo/([^/]+)" => "FooController",
        "/bar/([^/]+)" => "BarController@baz",
    };
    $app->addRoutes($routes);
    // Add a single route
    $app->addRoute("/bang/([^/]+)", function(Ui $ui, Log $log) {
      $log->write("Bang!");
      return "Bang!";
    });
    
    return $app;
  }
}
