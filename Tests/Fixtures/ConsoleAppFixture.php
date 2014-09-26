<?hh // strict
use Decouple\Console\Request\Request;
use Decouple\Console\CLI\CLI;
use Decouple\Console\Command\AbstractCommand;
use Decouple\Console\Command\CommandInterface;
class TestConsoleService {}
class FooCommand extends AbstractCommand {
  public static string $name = "foo:bar";
  public function execute() : void {
    CLI::output("FooBar!");
  }
}
class BarCommand extends AbstractCommand {
  public static string $name = "bar:baz";
  public function __construct(protected Request $request) {
    parent::__construct();
  }
  public function execute() : void {
    $arg = $this->request->getArg('bar')->getValue();
    CLI::output('bar:' . (string)$arg);
  }
}
