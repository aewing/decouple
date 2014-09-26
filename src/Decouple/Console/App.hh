<?hh // strict
namespace Decouple\Console;
use Decouple\Console\Request\Request;
use Decouple\Decoupler\Decoupler;
use Decouple\Console\Command\CommandInterface;
class App {
  protected Map<string,mixed> $commands;
  public function __construct(protected Request $request, protected Decoupler $decoupler, ?Vector<string> $commands=null) {
    $this->commands = Map {'decouple:version'=>'Decouple\Console\Command\VersionCommand'};
    if(!is_null($commands)) {
      $this->registerCommands($commands);
    }
  }
  public function execute() : mixed {
    $command = $this->request->getArg(0)->getVariable();
    $command = $this->getCommand($command);
    if($command instanceof CommandInterface) {
      return $command->execute();
    }
    throw new \Exception(sprintf("Unable to inject command instance: %s", $command));
  }
  public function registerCommands(Vector<string> $commands) : void {
    foreach($commands as $command) {
      $this->registerCommand($command);
    }
  }
  public function registerCommand(mixed $command) : void {
    if(!$command instanceof CommandInterface && !class_exists($command)) {
      throw new Command\NotFoundException((string)$command);
    }
    if(!is_subclass_of($command, 'Decouple\Console\Command\CommandInterface')) {
      throw new Command\NotFoundException((string)$command);
    }
    $this->commands->set(call_user_func_array([$command,'getName'], []), $command);
  }
  public function getCommand(string $name) : mixed {
    if($this->commands->contains($name)) {
      $command = $this->commands->at($name);
      return $this->decoupler->inject($command);
    }
    throw new Command\NotFoundException($name);
  }
}
