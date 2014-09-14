<?hh // strict
namespace Decouple\ORM;
use PDO;
class Query {
  private string $action;
  private Map<string,mixed> $data;
  private Vector<mixed> $selectFields;
  private Vector<Vector<mixed>> $whereData;
  public ?\PDOStatement $query;
  public ?string $query_raw;
  public function __construct(public string $table='', public string $driver='mysql')
  {
    $this->selectFields = Vector {};
    $this->data = Map {};
    $this->whereData = Vector {};
    $this->action = 'select';
  }
  public function select(?Vector<mixed> $fields=null) : Query {
    $this->action = 'select';
    if(is_null($fields)) {
      $fields = Vector {"*"};
    }
    if(!is_null($fields)) {
      $this->selectFields->addAll($fields);
    }
    return $this;
  }
  public function update(Map<string,mixed> $data) : Query {
    $this->action = 'update';
    $data['modified'] = time();
    $this->data->setAll($data);
    return $this;
  }
  public function delete(bool $soft=false) : Query {
    if(!$soft) {
      $this->action = 'delete';
      return $this;
    } else {
      $this->action = 'update';
      $data = Map { "deleted" => time() };
      $this->data->setAll($data);
      return $this;
    }
  }
  public function insert(Map<string,mixed> $data) : Query\Prepared {
    $this->action = 'insert';
    $this->data->setAll($data);
    return $this->buildQuery();
  }
  public function where(string $field, string $comp, mixed  $value) : Query {
    $this->whereData->add(Vector{$field, $comp, $value});
    return $this;
  }
  public function whereAll(KeyedTraversable<string,string> $array) : Query
  {
    foreach($array as $key => $where) 
    {
      if(count($where) == 3) {
        $this->where($where[0],$where[1],$where[2]);
      } else {
        if(is_string($key) && (is_string($where) || is_numeric($where))) {
          $this->where($key,'=',$where);
        }
      }
    }
    return $this;
  }
  public function buildQuery() : Query\Prepared {
    $query = '';
    if($this->action == 'select') {
      $query = 'SELECT ' . implode(',', $this->selectFields) . ' FROM ' . $this->table;
    } else if($this->action == 'update') {
      $query = 'UPDATE ' . $this->table . ' SET ';
      foreach($this->data as $var => $val) {
        if($var !== 'modified') {
          $query .= $var .' = :' . $var . ', ';
        }
      }
      if($this->driver == 'sqlite') {
        $query .= 'modified = strftime("%s", :modified)';
      } else {
        $query .= 'modified = FROM_UNIXTIME(:modified)';
      }
    } else if($this->action == 'insert') {
      $columns = [];
      $values = [];
      foreach($this->data as $var => $val) {
        $columns[] = $var;
        $values[] = ':' . $var;
      }
      $columns = implode(',', $columns);
      $values = implode(',', $values);
      $query = sprintf('INSERT INTO %s (%s) VALUES(%s)', $this->table, $columns, $values);
    } else if($this->action == 'delete') {
      $query = 'DELETE FROM ' . $this->table;
    }

    $whereFields = Map {};
    if(count($this->whereData) > 0) {
      $query .= ' WHERE ';
      foreach($this->whereData as $vec) {
        $value = $vec[2];
        if(is_null($value)) {
          $val = null;
        } else if(is_array($value)) {
          $val = json_encode($value);
        } else if(is_object($value) && $value instanceof Query\Raw) {
          $val = $value->getValue();
        } else {
          $val = $value;
        }
        $whereFields[(string)$vec[0]] = $val;

        $query .= (string)$vec[0] . ' ' . (string)$vec[1] . ' :' . (string)$vec[0] . ' AND ';
      }
      $query = substr($query,0,-4);
    }
    $fields = Map {};
    $fields->setAll($whereFields);
    $fields->setAll($this->data);
    return new Query\Prepared($query, $fields);
  }
  public function raw(mixed $value) : Query\Raw {
    return new Query\Raw($value);
  }
  public function reset() : void
  {
    $this->selectFields = Vector {};
    $this->data = Map {};
    $this->whereData = Vector {};
    $this->action = 'select';
  }
}
