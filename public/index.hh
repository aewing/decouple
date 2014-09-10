<?hh // partial
// Handle errors
set_error_handler(function (int $errno , string $errstr , string $errfile , int $errline) : bool {
  echo "[#" . $errno . "]\t" . $errstr . " (in " . $errfile . " on line #" . $errline . ")";
});
set_exception_handler(function (Exception $exception) {
  echo "Uncaught exception: " . $exception->getMessage() . "\n";
});

require_once dirname(__FILE__) . '/../vendor/autoload.php';
require_once dirname(__FILE__) . '/../app/bootstrap/http.hh';
