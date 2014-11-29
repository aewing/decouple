<?hh // strict
namespace Decouple\ORM\Table\Create;
class Field {
  public Map<string,mixed> $attributes;
  public function __construct(public string $name, public string $type) {
    $this->attributes = Map {};
  }
  public function length(int $length) : Field {
    $this->attributes->set('length', $length);
    return $this;
  }
  public function unsigned(bool $unsigned=true) : Field {
    $this->attributes->set('unsigned', $unsigned);
    return $this;
  }
  public function nullable(bool $nullable=true) : Field {
    $this->attributes->set('nullable', $nullable);
    return $this;
  }
  public function unique(bool $unique=true) : Field {
    $this->attributes->set('unique', $unique);
    return $this;
  }
  public function setDefault(mixed $default) : Field {
    if($default === null) {
      $default = '`NULL`';
    }
    $this->attributes->set('default', $default);
    return $this;
  }
  public function __call(string $name, array<mixed> $arguments) : mixed {
    if($name === 'default') {
      $meth = inst_meth($this, 'setDefault');
      return $meth($arguments[0]);
    }
  }
  public function getAttribute(string $name) : mixed {
    return $this->attributes->get($name);
  }
  public function setAttribute(string $name, mixed $value) : Field {
    $this->attributes->set($name, $value);
    return $this;
  }
}
