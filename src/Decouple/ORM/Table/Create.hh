<?hh // strict
namespace Decouple\ORM\Table;
use Decouple\ORM\Connector;
use Decouple\ORM\Query;
class Create {
  private Map<string,Create\Field> $map;
  private bool $timestamps;
  private bool $softDeletes;
  public function __construct(protected string $name, protected Connector $connector)
  {
    $this->map = Map {};
    $this->timestamps = false;
    $this->softDeletes = false;
  }
  public function integer(string $name, int $length=11) : Create\Field
  {
    $field = new Create\Field($name, 'int');
    $field->length($length);
    $this->map->set($name, $field);
    return $field;
  }
  public function string(string $name, int $length=55) : Create\Field
  {
    $field = new Create\Field($name, 'varchar');
    $field->length($length);
    $this->map->set($name, $field);
    return $field;
  }
  public function text(string $name) : Create\Field
  {
    $field = new Create\Field($name, 'text');
    $this->map->set($name, $field);
    return $field;
  }
  public function enum(string $name, Traversable<string> $values) : Create\Field
  {
    $field = new Create\Field($name, 'enum');
    $field->setAttribute('values', $values);
    $this->map->set($name, $field);
    return $field;
  }
  public function identifier(string $name='id', int $length=11) : Create\Field
  {
    $field = new Create\Field($name, 'id');
    $field->length($length);
    $this->map->set($name, $field);
    return $field;
  }
  public function foreign(string $name, string $target='', string $target_table='') : Create\Field
  {
    $field = new Create\Foreign($name, 'foreign');
    $field->setAttribute('target', $target);
    $field->setAttribute('target_table', $target_table);
    $this->map->set($name, $field);
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
  public function toString() : string {
    return serialize($this->map);
  }
  public function fromString(string $string) : void {
    $this->map = new Map(unserialize($string));
  }
  public function sql() : string  {
    $definition = "";
    $fields = [];
    $after = "";
    foreach($this->map as $field) {
      $type = "VARCHAR(255)";
      $fields[] = $field->name;
      $extras = [];

      if($field->getAttribute('default')) {
        $value = $field->getAttribute('default');
        if(is_string($value) && (substr($value, 0, 1) !== '`' || substr($value, -1, 1) !== '`')) {
          $value = "'" . (string)addslashes($value) . "'";
        } else {
          $value = (string)str_replace('`', '', $value);
        }
        $extras[] = sprintf('DEFAULT %s', $value);
      }


      if($field->type == "int") {
        if($this->connector->driver() == "sqlite") {
          $type = "INTEGER";
        } else {
          $type = sprintf("INT(%s)", $field->getAttribute('length')); 
        }
      } else if($field->type == "id") {
        if($this->connector->driver() !== "sqlite") {
          $type = sprintf("INT(%s)", $field->getAttribute('length')); 
          $extras[] = "AUTO_INCREMENT";
          $after = sprintf("PRIMARY KEY (%s)", $field->name);
        } else {
          $type = "INTEGER";
          $extras[] = "PRIMARY KEY";
        }
      } else if($field->type == "varchar") {
        if($this->connector->driver() == "sqlite") {
          $type = "TEXT";
        } else {
          $type = sprintf("VARCHAR(%s)", $field->getAttribute('length')); 
        }
      } else if($field->type == "text") {
        $type = "TEXT";
      } else if($field->type == "enum") {
        if($this->connector->driver() !== "sqlite") {
          $type = sprintf("ENUM('%s')", implode("','", $field->getAttribute('values')));
        } else {
          $type = "TEXT";
          $extras[] = sprintf("CHECK ( %s IN ('%s') )", $field->name, implode("','", $field->getAttribute('values')));
        }
      }

      // Handle signed vs unsigned
      if($field->getAttribute('unsigned')) {
        $extras[] = 'UNSIGNED';
      }
      // Handle nullability
      $null = '';
      if($field->getAttribute('nullable')) {
        $null = 'NULL';
      } else {
        if($this->connector->driver() != "sqlite" || $field->type != "enum") {
          $null = 'NOT NULL';
        }
      }
      // Unique 
      if($field->getAttribute('unique')) {
        $extras[] = 'UNIQUE';
      }

      
      $definition .= "\n" . sprintf("%s %s %s %s", $field->name, $type, $null, implode(' ', $extras)) . ",";
    }

    if($this->timestamps) {
      if($this->connector->driver() == "sqlite") {
        $definition .= "\ncreated TIMESTAMP DEFAULT CURRENT_TIMESTAMP,";
        $definition .= "\nmodified TIMESTAMP NULL DEFAULT NULL,";
        $definition .= "\ndeleted TIMESTAMP NULL DEFAULT NULL,";
      } else {
        $definition .= "\ncreated TIMESTAMP NOT NULL DEFAULT NOW(),";
        $definition .= "\nmodified TIMESTAMP NULL DEFAULT NULL,"; 
        $definition .= "\ndeleted TIMESTAMP NULL DEFAULT NULL,";
      }
    }

    $definition = substr($definition, 0,-1);

    $definition = sprintf("%s\n%s\n", $definition, $after);
    $fields = implode(',', $fields);
    $query = sprintf('CREATE TABLE `%s` (%s)', $this->name, $definition);
    return $query;
  }
}
