<?hh // partial
// Handle errors
set_error_handler(function (int $errno , string $errstr , string $errfile , int $errline) : bool {
  if($errno >= 2047) {
    echo "[#" . $errno . "]\t" . $errstr . " (in " . $errfile . " on line #" . $errline . ")";
  }
});
// Handle exceptions
set_exception_handler(function (Exception $exception) {
  echo "Uncaught exception: " . $exception->getMessage() . " in " . $exception->getFile() . " on line #" . $exception->getLine() . "\n";
});
