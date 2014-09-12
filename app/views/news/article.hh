<?hh // decl
class :news:article extends :ui:base {
  attribute
    string title = "",
    string image = "",
    string content = "",
    string date = "";

  public function compose() 
  {
    return 
      <div class="item">
        <div class="ui large rounded image">
          <img src={$this->getAttribute('image')}/>
          <a class="like ui corner label">
            <i class="like icon"></i>
          </a> 
        </div>
        <h4 class="header"></h4>
        <div class="content">
          <div class="name">{$this->getAttribute('title')}</div>
          <p class="description">{$this->getAttribute('content')}</p>
          <div class="extra">{$this->getAttribute('date')}</div>
        </div>
      </div>
    ;
  }
}
