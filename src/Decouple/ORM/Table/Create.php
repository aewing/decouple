<?hh // strict
namespace Decouple\ORM\Table;
class Create {
  private Map<string,CreateField> $map;
  private string $name;
  private bool $timestamps;
  private bool $softDeletes;
  public function __construct(string $name)
  {
    $this->name = $name;
    $this->map = Map {};
    $this->timestamps = false;
    $this->softDeletes = false;
  }
  public function integer(string $name, int $length=14) : CreateField
  {
    $field = new CreateField($name, 'integer');
    $field->length($length);
    $this->map->set($name, $field);
    return $field;
  }
  public function string(string $name, int $length=55) : CreateField
  {
    $field = new CreateField($name, 'varchar');
    $field->length($length);
    $this->map->set($name, $field);
    return $field;
  }
  public function text(string $name) : CreateField
  {
    $field = new CreateField($name, 'text');
    $this->map->set($name, $field);
    return $field;
  }
  public function foreign(string $name, string $target='', string $target_table='') : CreateField
  {
    $field = new CreateField($name, 'foreign');
    $this->map->set($name, $field);
    $field->set('target', $target);
    $field->set('target_table', $target_table);
    return $field;
  }
  public function timestamps(bool $is=true) : void
  {
    $this->timestamps = $is;
  }
  public function softDeletes(bool $is=true) : void
  {
    $this->softDeletes = $is;
  }
  public function execute() : Map<string,CreateField>
  {
    return $this->map;
  }
}

class CreateField {
  private Map<string,mixed> $config;
  public string $type;
  public string $name;
  public function __construct(string $name, string $type)
  {
    $this->name = $name;
    $this->type = $type;
    $this->config = Map {};
  }
  public function length(int $length) : CreateField
  {
    $this->config->set('length', $length);
    return $this;
  }
  public function unsigned(bool $is=true) : CreateField
  {
    $this->config->set('unsigned', $is);
    return $this;
  }
  public function set(string $attrib, mixed $value) : CreateField
  {
    $this->config->set($attrib, $value);
    return $this;
  }
  public function references(string $field) : CreateField
  {
    /**
     * TODO: Make sure field is either foreign or unsignedint & set to foreign
     */
    $this->config->set('target', $field);
    return $this;
  }
  public function on(string $table) : CreateField
  {
    /**
     * TODO: Make sure field is foreign
     */
    $this->config->set('target_table', $table);
    return $this;
  }
  public function unique(bool $is) : CreateField
  {
    $this->config->set('unique', $is);
    return $this;
  }
}
