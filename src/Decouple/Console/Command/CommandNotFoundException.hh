<?hh // strict
namespace Decouple\Console\Command;
use Exception;
class NotFoundException extends Exception {
  public function __construct(string $name) {
    $message = sprintf("Command not found: %s", $name);
    parent::__construct($message);
  }
}
