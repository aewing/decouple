<?hh // strict
namespace Decouple\Log;
class Log {
  private resource $stream;
  public function __construct(private string $path) {
    if(!is_writable($path)) {
      throw new \Exception(sprintf("Invalid/unwritable log path provided: %s", $path));
    }
    $this->stream = fopen($path, 'a+');
  }
  public function write(string $message) : void {
    if(!stristr("\n", $message)) {
      $message .= "\n";
    }
    fwrite($this->stream, $message);
  }
  public function __destruct() {
    fclose($this->stream);
  }
}
