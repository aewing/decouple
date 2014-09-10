<?hh // partial 
use Decouple\Log\Log;
use Decouple\Ui\Ui;
// Load application services from a map
$services = Map {
  "Decouple\Log\Log" => new Log((string)$paths->get('app.logs') . "/http.log"),
  "Decouple\Ui\Ui" => true
};
$app->addServices($services);
