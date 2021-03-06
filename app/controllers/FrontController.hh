<?hh // strict
use Decouple\Ui\Ui;
use Decouple\ORM\Connector;
use Decouple\ORM\Query\Raw;
class FrontController {
  public function index(Connector $db, DebugRegistry $debug) : string {
    $view = <front:index></front:index>;
    $news = $db->table('news');
    $news->insert([
      'title' => 'Article C',
      'content' => 'Another article content',
      'author' => 1,
      'image' => '',
      'create_date' => new Raw('CURRENT_TIMESTAMP')
    ]);

    $articles = $news
      ->select(Vector {'id','name','content','author','create_date','image'})
      ->where('delete_date', 'is', new Raw('NULL'))
      ->fetchAll()
    ;

    $content = <div/>;
    if(is_null($articles)) {
      $error =
        <div class="ui error message">
          <h2>Oops!</h2>
          <p>I couldn't find any news articles to display... Try again?</p>
        </div>;
      $content->appendChild($error);
    } else {
      $container = <news:articles/>;
      foreach($articles as $article) {
        $body = <news:article/>;
        $body->setAttribute('title', $article['name']);
        $body->setAttribute('content', $article['content']);
        $body->setAttribute('date', $article['create_date']);
        $body->setAttribute('image', $article['image']);
        $container->appendChild($body);
      }
      $content->appendChild($container);
    }
    $view->appendChild($content);
    $view->appendChild(<br/>);
    $view->appendChild(<hr/>);
    $view->appendChild(<br/>);
    $view->appendChild(perf_info($debug));

    return (string)$view;
  }

  public function test() : string {
    return "Test";
  }
}
