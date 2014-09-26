<?hh // strict
newtype ArgumentType = int;
namespace Decouple\Console\Request;
use ArgumentType;
class Argument {
  const ArgumentType ARGUMENT = 0;
  const ArgumentType FLAG = 1;
  const ArgumentType OPTION = 2;
  const ArgumentType OTHER = 3;
  protected ArgumentType $type;
  protected string $variable;
  protected mixed $value = false;
  public function __construct(string $text) {
    if(substr($text, 0, 2) == '--') {
      $text = substr($text, 2);
      $this->type = Argument::OPTION;
      if (stristr($text,'=')) {
        list($this->variable, $this->value) = explode('=', $text);
      } else {
        $this->variable = $text;
        $this->value = true;
      }
    } else if(substr($text, 0, 1) == '-') {
      $this->variable = substr($text, 1, 0);
      $this->value = true;
      $this->type = Argument::FLAG;
    } else {
      $this->variable = $text;
      $this->value = null;
      $this->type = Argument::ARGUMENT;
    }
  }
  public function getType() : ArgumentType {
    return $this->type;
  }
  public function getVariable() : string {
    return $this->variable;
  }
  public function getValue() : mixed {
    return $this->value;
  }
}
