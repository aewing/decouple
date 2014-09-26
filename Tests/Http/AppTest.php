<?hh // partial 
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Request\Request;
use Decouple\HTTP\Request\Uri;
use Decouple\Ui\Ui;
use Decouple\HTTP\Router\Router;
use Decouple\HTTP\App;
use Decouple\Registry\Paths;
use Decouple\Log\Log;
require_once dirname(__FILE__) . '/../Fixtures/HTTPAppFixture.php';
class HTTP_AppTest extends PHPUnit_Framework_TestCase {
  public function testAppBootstrapA() : void {
    $app = $this->__bootstrap("/foo/bar");
    $result = $app->execute();
    $this->assertEquals($result, 'FooBar!');
  }
  public function testAppBootstrapB() : void {
    $app = $this->__bootstrap("/bar/baz");
    $result = $app->execute();
    $this->assertEquals($result, 'baz');
  }
  public function testAppBootstrapC() : void {
    $app = $this->__bootstrap("/bang/bam");
    $result = $app->execute();
    $this->assertEquals($result, 'Bang!');
  }
  public function __bootstrap(string $uri) : App {
    // HTTP request
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
      "Decouple\HTTP\Request\Request" => $request,
      "Decouple\HTTP\Request\Uri" => $uri,
      "Decouple\Registry\Paths" => $paths,
      "Decouple\Ui\Ui" => new Decouple\Ui\Ui(),
    });

    // Router
    $router = new Router($decoupler);

    // HTTP application
    $app = new App($request, $router, $decoupler);

    // Load application services from a map
    $services = Map {
      "Decouple\Log\Log" => new Log((string)$paths->get('app.logs') . "/http.log"),
      "TestHTTPService" => true
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
