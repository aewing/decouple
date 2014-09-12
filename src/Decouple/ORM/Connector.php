<?hh // strict 
namespace Decouple\ORM;
use PDO;
class Connector {
  private bool $connected;
  private PDO $pdo;
  public function __construct(string $dsn, string $username, string $password)
  {
      $this->pdo = new PDO($dsn, $username, $password);
      $this->connected = false;
  }
  public function pdo() : \PDO
  {
    if(!$this->connected) {
    }
    if(!$this->pdo) {
      throw new \Exception("Unable to connect to database");
    }
    $this->connected = true;
    return $this->pdo;
  }
  public function query(string $table) : Query {
    return new Query($this->pdo, $table);
  }
  public function table(string $table) : Table {
    return new Table($this->pdo, $table);
  }
}
