<?hh // strict
namespace Decouple\ORM;
use PDO;
class Table {
  public function __construct(private PDO $pdo, public string $name) {}
  public function select(?Vector<string> $fields=null) : mixed  {
    $query = new Query($this->pdo, $this->name);
    return $query->select($fields);
  }
  public function create() : Table\Create {
    return new Table\Create($this->name);
  }
}
