import os

output_dir = os.getenv("DOXYGEN_OUTPUT_DIR")

if not output_dir:
    os.environ["DOXYGEN_OUTPUT_DIR"] = os.getenv("PWD") + "/doxygen"

os.system("cd ..; doxygen")

# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'CSP Fork (PIA)'
copyright = '%Y, G team'
author = ("Xibitol, Valentin GEGADEN, nandeee_753, Ryan Heuvel, Adrien "
          "Gueguen, LeGoblin")
release = 'v1.0.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'
# needs_sphinx = "8.1"

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
source_suffix = [".rst", ".md"]
# source_suffix = '.rst'

# The master toctree document.
master_doc = "index"

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ["sphinx.ext.imgmath", "sphinx.ext.todo", "breathe", "recommonmark"]
breathe_projects = {"myproject": os.getenv("DOXYGEN_OUTPUT_DIR") + "/xml/"}
breathe_default_project = "myproject"


# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = "en"

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# The name of the Pygments (syntax highlighting) style to use.
pygments_style = "sphinx"


todo_include_todos = False




# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "sphinx_rtd_theme"

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
# html_static_path = ['_static']

intersphinx_mapping = {"https://docs.python.org/": None}
