<?hh // strict
namespace Decouple\ORM;
use PDO;
class Table {
  public function __construct(public string $name, public string $driver="mysql") {}
  public function select(?Vector<mixed> $fields=null) : Query {
    $query = new Query($this->name, $this->driver);
    return $query->select($fields);
  }
  public function create() : Table\Create {
    return new Table\Create($this->name, $this->driver);
  }
  public function insert(array<string,mixed> $data) : Query\Prepared {
    $query = new Query($this->name, $this->driver);
    return $query->insert(Map::fromArray($data));
  }
}
