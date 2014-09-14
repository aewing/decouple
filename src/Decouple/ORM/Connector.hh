<?hh // strict 
namespace Decouple\ORM;
use PDO;
class Connector {
  protected bool $connected;
  protected PDO $pdo;
  public function __construct(string $dsn, ?string $username=null, ?string $password=null)
  {
      $this->pdo = new PDO($dsn, $username, $password);
      $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      $this->connected = ($this->pdo) ? true : false;
  }
  public function pdo() : PDO {
    return $this->pdo;
  }
  public function driver() : string {
    return $this->pdo->getAttribute(PDO::ATTR_DRIVER_NAME);
  }
  public function query(string $table='') : Query {
    return new Query($table, $this->driver());
  }
  public function table(string $table) : Table {
    return new Table($table, $this->driver());
  }
  public function insert(string $table, array<string,mixed> $data, bool $row=false) : mixed {
    $table = new Table($table, $this->driver());
    $prepared = $table->insert($data);
    $this->statement($prepared->query)->execute($prepared->values);
    $id = $this->pdo->lastInsertId();
    if($row) {
      $query = $table->select()->where('id','=',$id);
      $fetched = $this->fetch($query);
      return $fetched;
    } else {
      return $id;
    }
  }
  public function fetchAll(Query $query) : Vector<Map<string,mixed>> {
    $sql = $query->buildQuery();
    $statement = $this->statement($sql->query);
    $statement->execute($sql->values);
    $return = Vector {};
    $results = $statement->fetchAll();
    foreach($results as $result) {
      $return->add(Map::fromArray($result));
    }
    return $return;
  }
  public function fetch(Query $query) : Map<string,mixed> {
    $sql = $query->buildQuery();
    $statement = $this->statement($sql->query);
    $statement->execute($sql->values);
    $result = $statement->fetch();
    return Map::fromArray($result);
  }
  public function fetchColumn(Query $query, int $column=0) : mixed {
    $sql = $query->buildQuery();
    $statement = $this->statement($sql->query);
    $statement->execute($sql->values);
    return $statement->fetchColumn($column);
  }
  public function statement(string $query) : \PDOStatement {
    return $this->pdo->prepare($query);
  }
  public function execute(mixed $sql, bool $check=false) : mixed {
    if($sql instanceof Query) {
      $statement = $sql->buildQuery();
      return $this->statement($statement->query)->execute($statement->values);
    } else if($sql instanceof Query\Prepared) {
      return $this->statement($sql->query)->execute($sql->values);
    }
    if(!is_string($sql)) {
      throw new \Exception(sprintf("Invalid query provided to Decouple\ORM\Connector::execute, expected string; %s", $sql));
    }
    try {
      $affected = $this->pdo->exec($sql);
    } catch(\Exception $e) {
      if($check) {
        return false;
      } else {
        throw $e;
      }
    }
    return $check ? true : $affected;
  }
  public function error() : mixed {
    return $this->pdo->errorInfo();
  }
  public function connected() : bool {
    return $this->connected;
  }
}
