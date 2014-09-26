<?hh // strict
namespace Decouple\Console\Command;
interface CommandInterface {
  public function execute() : mixed;
  public static function getName() : string;
}
