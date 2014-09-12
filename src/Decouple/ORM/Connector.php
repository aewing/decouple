<?hh // strict 
namespace Decouple\ORM;
use PDO;
class Connector {
  protected bool $connected;
  protected PDO $pdo;
  public function __construct(string $dsn, ?string $username=null, ?string $password=null)
  {
      $this->pdo = new PDO($dsn, $username, $password);
      $this->connected = ($this->pdo) ? true : false;
  }
  public function query(string $table) : Query {
    return new Query($this->pdo, $table);
  }
  public function table(string $table) : Table {
    return new Table($this->pdo, $table);
  }
  public function connected() : bool {
    return $this->connected;
  }
}
