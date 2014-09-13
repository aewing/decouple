<?hh // strict
namespace Decouple\ORM\Table\Create;
class Foreign extends Field {
  public function references(string $field) : Field 
  {
    $this->setAttribute('target', $field);
    return $this;
  }
  public function on(string $table) : Field
  {
    $this->setAttribute('target_table', $table);
    return $this;
  }
}
