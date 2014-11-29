<?hh // decl 
namespace Decouple\Autoload;
class Autoload
{
  protected Map<string,Vector<string>> $prefixes;
  public function __construct(public Map <string,Vector<string>> $paths=Map {})
  {
    $this->prefixes = Map {};
  }

  public function register() : void
  {
    spl_autoload_register([$this, 'loadClass']);
  }

  public function addNamespace(string $prefix, string $dir, bool $prepend=false) : void
  {
    // Normalize prefix
    $prefix = trim($prefix, '\\') . '\\';

    // Normalize base directory
    $dir = rtrim($dir, DIRECTORY_SEPARATOR) . '/';

    if(!$this->prefixes->contains($prefix)) {
      $vector = Vector {};
      $this->prefixes->set($prefix, $vector);
    } else {
      $vector = $this->prefixes->get($prefix);
    }

    if($prepend) {
      $vector->reverse();
      $vector->add($dir) ;
      $vector->reverse();
    } else {
      $vector->add($dir);
    }
  }

  /** Load a class **/
  public function loadClass(string $class) : mixed 
  {
    if(substr($class,0,4) == 'xhp_') {
      $path = $this->paths->get('views') . '/' . implode('/', explode('__', substr($class, 4))) . '.hh';
      if(file_exists($path)) {
        return require($path);
      }
      return null;
    }

    $prefix = $class;
    // work backwards through the namespace names of the fully-qualified
    // class name to find a mapped file name.
    while (false !== $pos = strrpos($prefix, '\\')) {

      // retain the trailing namespace separator in the prefix
      $prefix = substr($class, 0, $pos + 1);

      // the rest is the relative class name
      $relative_class = substr($class, $pos + 1);

      // try to load a mapped file for the prefix and relative class
      $mapped_file = $this->loadMappedFile($prefix, $relative_class);
      if ($mapped_file) {
        return $mapped_file;
      }

      // remove the trailing namespace separator for the next iteration
      // of strrpos()
      $prefix = rtrim($prefix, '\\');   
    }

    // never found a mapped file
    return null;
  }
  /**
     * Load the mapped file for a namespace prefix and relative class.
     * 
     * @param string $prefix The namespace prefix.
     * @param string $relative_class The relative class name.
     * @return mixed Boolean false if no mapped file can be loaded, or the
     * name of the mapped file that was loaded.
     */
    protected function loadMappedFile(string $prefix, string $relative_class) :?string 
    {
        // are there any base directories for this namespace prefix?
        if (!$this->prefixes->contains($prefix)) {
            return null;
        }

        // look through base directories for this namespace prefix.
        $vector = $this->prefixes->get($prefix);
        foreach ($vector as $base_dir) {

            // replace the namespace prefix with the base directory,
            // replace namespace separators with directory separators
            // in the relative class name, append with .php
            $file = $base_dir
                  . str_replace('\\', '/', $relative_class);

            // if the mapped file exists, require it
            if ($this->requireFile($file)) {
                // yes, we're done
                return $file;
            }
        }

        // never found it
        return null;
    }

    /**
     * If a file exists, require it from the file system.
     * 
     * @param string $file The file to require.
     * @return bool True if the file exists, false if not.
     */
    protected function requireFile(string $file) : mixed 
    {
      echo "Checking for file: " . $file . "\n";
        if (file_exists($file . '.hh')) {
          return require $file . '.hh';
        } elseif (file_exists($file . '.php')) {
          return require $file . '.php';
        }
        return false;
    }
}
