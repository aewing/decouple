<?hh // strict
namespace Decouple\Console\CLI;
use STDIN;
class CLI {
  public static function output(string $message) : void {
    echo $message . "\n";
  }
}
