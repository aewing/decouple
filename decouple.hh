<?hh // partial
require_once dirname(__FILE__) . "/vendor/autoload.php";

class Logger {
  public function out($message) : void {
    echo $message . "\n";
  }
}

class Controller {
  public function index(Logger $log) : void {
    $log->out("Controller log");
  }
}

$services = [
  "Logger" => new Logger()
];

$controller = new Controller();
$decoupler = new Decouple\Decoupler\Decoupler(Map::fromArray($services));
decouple_object_method($decoupler, $controller, 'index');
