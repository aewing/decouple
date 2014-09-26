<?hh // strict
namespace Decouple\Console\Command;
use Decouple\Console\CLI\CLI;
class VersionCommand extends AbstractCommand implements CommandInterface {
  public static string $name = 'decouple:version';
  public function execute() : void {
    CLI::output(sprintf("Decouple v%s", "0.1a"));
  }
}
