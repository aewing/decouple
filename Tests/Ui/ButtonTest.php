<?hh // partial 
use Decouple\Ui\Ui;
class ButtonTest extends PHPUnit_Framework_TestCase {
  public function testButton() : void {
    $button = Ui::autoload();
    $button = <ui:button></ui:button>;
    $button->setAttribute('size', 'large');
    $button->setAttribute('color', 'green');
    $markdown = $button->__toString();
    $this->assertEquals($markdown, '<button class="large green"></button>');
  }
}
