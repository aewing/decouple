<?hh // strict
namespace Decouple\ORM;
use PDO;
use PDOStatement;
class Query {
  private string $action;
  private Map<string,mixed> $data;
  private Vector<mixed> $selectFields;
  private Vector<Vector<mixed>> $whereData;
  public ?\PDOStatement $query;
  public ?string $query_raw;

  public function __construct(public string $table, protected Connector $connector)
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

  public function insert(Map<string,mixed> $data) : ?Map<string,mixed> {
    $this->action = 'insert';
    $this->data->setAll($data);
    try { 
      $this->execute();
      $id = $this->connector->pdo()->lastInsertId();
      if($id) {
        $this->reset();
        return $this->select()->where('id','=',$id)->fetch();
      }
      return null;
    } catch(\Exception $e) {
      return null;
    }
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

  public function build() : Query\Prepared {
    $query = '';
    if($this->action == 'select') {
      $query = 'SELECT ' . implode(',', $this->selectFields) . ' FROM ' . $this->table;
    } else if($this->action == 'update') {
      $query = 'UPDATE ' . $this->table . ' SET ';
      foreach($this->data as $var => $val) {
        if($val instanceof \Decouple\ORM\Query\Raw) {
          $query .= $var .' = ' . (string)$val->getValue();
        } elseif($var !== 'modified') {
          $query .= $var .' = :' . $var . ', ';
        }
      }
      if($this->connector->driver() == 'sqlite') {
        $query .= 'modified = strftime("%s", :modified)';
      } else {
        $query .= 'modified = FROM_UNIXTIME(:modified)';
      }
    } else if($this->action == 'insert') {
      $columns = [];
      $values = [];
      foreach($this->data as $var => $val) {
        $columns[] = $var;
        if($val instanceof \Decouple\ORM\Query\Raw) {
          $values[] = $val->getValue();
        } else {
          $values[] = ':' . $var;
        }
      }
      $columns = implode(',', $columns);
      $values = implode(',', $values);
      $query = 'INSERT INTO ' . $this->table . ' (' . $columns  . ') VALUES(' . $values  . ')';
    } else if($this->action == 'delete') {
      $query = 'DELETE FROM ' . $this->table;
    }

    $whereFields = Map {};
    if(count($this->whereData) > 0) {
      $query .= ' WHERE ';
      foreach($this->whereData as $vec) {
        $field = (string)$vec[0];
        $operator = (string)$vec[1];
        $value = $vec[2];
        $standIn = ':' . $field;
        if(is_null($value)) {
          $val = null;
        } elseif(is_array($value)) {
          $val = json_encode($value);
        } elseif($value instanceof \Decouple\ORM\Query\Raw) {
          $val = $value->getValue();
          $standIn = (string)$val;
        } else {
          $val = $value;
        }
        $whereFields[(string)$vec[0]] = $val;

        $query .= $field . ' ' . $operator . ' ' . $standIn . ' AND ';
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

  public function fetch() : Map<string,mixed> {
    return Map::fromArray($this->execute()->fetch());
  }

  public function fetchAll() : Vector<Map<string,mixed>> {
    $fetched = $this->execute()->fetchAll();
    $result = Vector {};
    foreach($fetched as $row) {
      $result[] = Map::fromArray($row);
    }
    return $result;
  }

  public function fetchColumn(int $column=0) : mixed {
    return $this->execute()->fetchColumn($column);
  }

  public function execute() : PDOStatement {
    $build = $this->build();
    $statement = $this->connector->statement($build->query);
    $values = $build->values;
    foreach($values as $index => $value) {
      if($value instanceof \Decouple\ORM\Query\Raw) {
        unset($values[$index]);
      }
    }
    $statement->execute($values);
    return $statement;
  }
}
