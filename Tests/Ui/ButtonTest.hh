<?hh // strict
use HackPack\HackUnit\Core\TestCase;
use Decouple\Ui\Ui;
class ButtonTest extends TestCase {
  public function testButton() : void {
    $button = Ui::autoload();
    $button = <ui:button></ui:button>;
    $button->setAttribute('size', 'large');
    $button->setAttribute('color', 'green');
    $markdown = $button->__toString();
    $this->expect($markdown)->toEqual('<button class="large green"></button>');
  }
}
