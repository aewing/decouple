<?hh // partial 
use Decouple\Log\Log;
use Decouple\Ui\Ui;
use Decouple\ORM\Connector;
// Load application services from a map
$services = Map {
  "Decouple\Log\Log" => new Log((string)$paths->get('app.logs') . "/http.log"),
  "Decouple\Ui\Ui" => true,
  "Decouple\ORM\Connector" => new Connector('mysql:host=localhost;dbname=decouple;','homestead','secret')
};
$app->addServices($services);
