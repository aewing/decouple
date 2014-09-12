<?hh // partial
use Decouple\Http\Request\Uri;
use Decouple\Http\Request\Request;
use Decouple\Registry\Registry;
use Decouple\Registry\Paths;
use Decouple\Ui\Ui;
use Decouple\Decoupler\Decoupler;
use Decouple\Http\Router\Router;
use Decouple\Http\Http;
// Http request
$uri = new Uri($_SERVER['REQUEST_URI']);
$get = Map::fromArray($_GET);
$post = Map::fromArray($_POST);
$files = Map::fromArray($_FILES);
$request = new Request($uri, $get, $post, $files);

// Paths configuration
$root = (string)realpath((string)dirname(__FILE__) . '/../../');
$paths = new Paths(Map {
  "root" => $root,
  "app" => $root . '/app',
  "app.logs" => $root . '/app/logs',
  "ui" => $root . '/ui',
  "public" => $root . '/public'
});

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
$app = new Http($request, $router, $decoupler);

// Bootstrap
require_once $paths->get('app') . '/bootstrap/http.services.hh';
require_once $paths->get('app') . '/bootstrap/http.routes.hh';

echo $app->execute();
