<?hh // strict
namespace Decouple\ORM\Query;
class Prepared {
  public function __construct(public string $query, public Map<string,mixed> $values) {}
}
