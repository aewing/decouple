<?hh // strict
namespace Decouple\ORM;
use PDO;
class Query {
  public string $table;
  private string $action;
  private Vector<string> $updateFields;
  private Vector<string> $selectFields;
  private Vector<Vector<mixed>> $whereData;
  public ?\PDOStatement $query;
  public ?string $query_raw;
  private PDO $pdo;
  public function __construct(PDO $pdo, string $table)
  {
    $this->pdo = $pdo;
    $this->table = $table;
    $this->selectFields = Vector {};
    $this->updateFields = Vector {};
    $this->whereData = Vector {};
    $this->action = 'select';
  }
  public function select(?Vector<string> $fields=null) : Query {
    $this->action = 'select';
    if(is_null($fields)) {
      $fields = Vector {"*"};
    }
    $this->selectFields->addAll($fields);
    return $this;
  }
  public function update(?Vector<string> $fields=null) : Query {
    $this->action = 'update';
    $this->updateFields->setAll($fields);
    return $this;
  }
  public function where(string $field, string $comp, mixed $value) : Query {
    $this->whereData->add(Vector{$field,$comp,$value});
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
  public function fetch() : array<string,array<string,mixed>> {
    return $this->query()->fetchAll();
  }
  public function query() : \PDOStatement {
    $query = ($this->action == 'update') ? 'UPDATE' : 'SELECT';
    if($this->action == 'select') {
      $query = 'SELECT ' . implode(',', $this->selectFields) . ' FROM ' . $this->table;
    } else if($this->action == 'UPDATE') {
      $query = 'UPDATE ' . $this->table . ' SET ';
      foreach($this->updateFields as $var => $val) {
        $query .= $var .' = :' . $var . ', ';
      }
      $query .= 'modified = :modified';
    }
    $whereFields = [];
    if(!is_null($this->whereData)) {
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
        $whereFields[$vec[0]] = $val;

        $query .= (string)$vec[0] . ' ' . (string)$vec[1] . ' :' . (string)$vec[0] . ' AND ';
      }
      $query = substr($query,0,-4);
    }
    $this->query_raw = $query;
    $this->query = $query = $this->pdo->prepare($query);
    if($this->action == 'select') {
      $fields = $whereFields;
    } else {
      $fields = $this->selectFields;
      foreach($whereFields as $field) {
        $fields[] = $field;
      }
    }
    $query->execute($fields);
    return $query;
  }
  public function raw(mixed $value) : Query\Raw {
    return new Query\Raw($value);
  }
  public function reset() : void
  {
    $this->selectFields = Vector {};
    $this->updateFields = Vector {};
    $this->whereData = Vector {};
    $this->action = 'select';
  }
}
