<?hh // decl
abstract class :ui:base extends :x:element {

  abstract protected function compose() : :xhp;
 
  final public function addClass(string $class) : :ui:base {
    $this->setAttribute(
      'class',
      trim((string)$this->getAttribute('class').' '.$class)
    );
    return $this;
  }

  final protected function render() : :xhp {
    $root = $this->compose();
    if ($root === null) {
      return <x:frag />;
    }
    if (:x:base::$ENABLE_VALIDATION) {
      if (!$root instanceof :xhp:html-element
          && !$root instanceof :ui:base) {
        throw new XHPClassException(
          $this,
          'compose() must return an xhp:html-element'.
          ' or ui:base instance.'
        );
      }
    }

    // Get all attributes declared on this instance
    $attributes = Map::fromArray($this->getAttributes());
    // Get all allowed attributes on the node returned
    // from compose()
    $declared = Map::fromArray($root->__xhpAttributeDeclaration());

    // Transfer any classes that were added inline over
    // to the root node.
    if ($attributes->contains('class')) {
      $attributes['class'] && $root->addClass($attributes['class']);
      $attributes->remove('class');
    }

    // Always forward data and aria attributes
    $html5Attributes = Map::fromArray(array('data-' => true, 'aria-' => true));

    // Transfer all valid attributes to $root
    foreach ($attributes as $attribute => $value) {
      if ($declared->contains($attribute) ||
          $html5Attributes->contains(substr($attribute, 0, 5))) {
        try {
          $root->setAttribute($attribute, $value);
        } catch (XHPInvalidAttributeException $e) {
          // This happens when the attribute defined on
          // your instance has a different definition
          // than the one you've returned. This usually
          // happens when you've defined different enum values.
          // When you turn off validation (like on prod) these
          // errors will not be thrown, so you should
          // fix your APIs to use different attributes.
          error_log(
            'Attribute name collision for '.$attribute.
            ' in :ui:base::render() when transferring'.
            ' attributes to a returned node. source: '.
            (string)$this->source."\nException: ".$e->getMessage()
          );
        }
      }
    }

    return $root;
  }

}

