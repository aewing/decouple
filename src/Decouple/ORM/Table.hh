<?hh // strict
namespace Decouple\ORM;
class Table {
  public function __construct(public string $name, protected Connector $connector) {}
  public function select(?Vector<mixed> $fields=null) : Query {
    $query = new Query($this->name, $this->connector);
    return $query->select($fields);
  }
  public function create() : Table\Create {
    return new Table\Create($this->name, $this->connector);
  }
  public function insert(array<string,mixed> $data) : ?Map<string,mixed> {
    $query = new Query($this->name, $this->connector);
    return $query->insert(Map::fromArray($data));
  }
  public function update(array<string,mixed> $data) : Query {
    $query = new Query($this->name, $this->connector);
    return $query->update(Map::fromArray($data));
  }
  public function delete() : Query {
    $query = new Query($this->name, $this->connector);
    return $query->delete();
  }
}
