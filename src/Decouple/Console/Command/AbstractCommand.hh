<?hh // strict
namespace Decouple\Console\Command;
abstract class AbstractCommand implements CommandInterface {
  public static string $name = '';
  public function __construct() {}
  public static function getName() : string {
    return static::$name;
  }
  abstract public function execute() : mixed;
}
