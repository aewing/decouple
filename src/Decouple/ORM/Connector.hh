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
    return new Query($table, $this);
  }
  public function table(string $table) : Table {
    return new Table($table, $this);
  }
  public function statement(string $query) : \PDOStatement {
    return $this->pdo->prepare($query);
  }
  public function execute(string $sql, bool $check=false) : mixed {
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
