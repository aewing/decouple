<?hh // partial
require_once dirname(__FILE__) . '/http/errors.hh';
// Paths configuration
$root = (string)realpath((string)dirname(__FILE__) . '/../../');
$paths = Map {
  "root" => $root,
  "src" => $root . '/src',
  "app" => $root . '/app',
  "controllers" => $root . '/app/controllers',
  "bootstrap" => $root . '/app/bootstrap',
  "app.logs" => $root . '/app/logs',
  "views" => $root . '/app/views',
  "public" => $root . '/public'
};
require_once dirname(__FILE__) . '/http/autoload.hh';
require_once dirname(__FILE__) . '/xhp/init.hh';
use Decouple\HTTP\Request\Uri;
use Decouple\HTTP\Request\Request;
use Decouple\Registry\Registry;
use Decouple\Registry\Paths;
use Decouple\Ui\Ui;
use Decouple\Decoupler\Decoupler;
use Decouple\HTTP\Router\Router;
use Decouple\HTTP\App;
// Http request
$uri = new Uri($_SERVER['REQUEST_URI']);
$get = Map::fromArray($_GET);
$post = Map::fromArray($_POST);
$files = Map::fromArray($_FILES);
$request = new Request($uri, $get, $post, $files);


class DebugRegistry extends Registry {}
$debug = new DebugRegistry(Map {
  "start_time" => $start
});

// Decoupler configuration
$decoupler = new Decoupler(Map {
  "Decouple\Http\Request\Uri" => $uri,
  "Decouple\Http\Request\Request" => $request,
  "Decouple\Registry\Paths" => $paths,
  "DebugRegistry" => $debug
});

// Router
$router = new Router($decoupler);

// Http application
$app = new App($request, $router, $decoupler);

// Bootstrap
require_once $paths->get('app') . '/bootstrap/http/services.hh';
require_once $paths->get('app') . '/bootstrap/http/routes.hh';

echo $app->execute();
