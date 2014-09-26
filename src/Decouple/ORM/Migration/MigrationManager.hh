<?hh // strict
namespace Decouple\ORM\Migration;
use Decouple\ORM\Migration\Migration;
use Decouple\ORM\Table\Create;
class MigrationManager {
  public static function diff(Migration $a, Migration $b) : Create
  {
    $new = Map {};
    $changed = Map {};
    foreach($b->getMap() as $name => $field) {
      if(!$a->contains($name)) {
        $new->set($name, $field);
      } else {
        if(serialize($a) !== serialize($b)) {
          $changed->set($name, 
        }
      }
    }
  }
}
