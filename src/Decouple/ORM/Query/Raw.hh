<?hh // strict
namespace Decouple\ORM\Query;
class Raw {
  public function __construct(protected mixed $value) {}
  public function getValue() : mixed {
    return $this->value;
  }
}
