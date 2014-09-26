<?hh // strict
namespace Decouple\Console\Request;
use ArgumentType;
class Request {
  protected Vector<Argument> $arguments;
  public function __construct(Vector<string> $args) {
    $this->arguments = Vector {};
    foreach($args as $arg) {
      $this->arguments->add(new Argument($arg));
    }
  }
  public function getOptions() : Vector<Argument> {
    return $this->getByType(Argument::OPTION);
  }
  public function getFlags() : Vector<Argument> {
    return $this->getByType(Argument::FLAG);
  }
  public function getArgs() : Vector<Argument> {
    return $this->getByType(Argument::ARGUMENT);
  }
  public function getByType(ArgumentType $type) : Vector<Argument> {
    $arguments = Vector {};
    foreach($this->arguments as $argument) {
      if($argument->getType() === $type) {
        $arguments->add($argument);
      }
    }
    return $arguments;
  }
  public function getArg(mixed $seek) : Argument {
    if(is_int($seek)) {
      if(!$this->arguments->containsKey($seek)) {
        throw new InvalidRequestException(sprintf("Argument not found at index %s", $seek));
      }
      return $this->arguments->at($seek);
    } else if(is_string($seek)) {
      foreach($this->arguments as $argument) {
        if($argument->getVariable() == $seek) {
          return $argument;
        }
      }
    }
    throw new InvalidRequestException(sprintf("Argument not found: %s", $seek));
  }
  public function at(int $index) : Argument {
    if(!$this->arguments->containsKey($index)) {
      throw new InvalidRequestException(sprintf("No argument found at index %s", $index));
    }
    return $this->arguments->at($index);
  }
}
