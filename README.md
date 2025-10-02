# UC3
## What Is UC3?
UC3 is an experimental configuration toolchain and DSL. At its core, it's meant to follow the purpose of similar systems like KConfig. UC3 breaks a program down into its subsystems, which each have configurable components. This separates subsystems into interchangable pieces of code. They can be enabled, disabled, or configured in any way. This is primarily done with the generation of C Preprocessor Macros.

Although UC3 is far from finished, it's rapidly approaching its first prototype. At this point in development, components can already declare their dependencies, being what other components they need to function. UC3 is already capable of analyzing these relationships, and graphing them. It is through this that UC3 has already proposed a prototype dependency verifier, which checks for circular dependencies.

## The UC3 DSL
UC3 employs its own Domain-Specific Language (DSL) to declare and define component, subsystems, and how they can be configured. The UC3 DSL is aims to be as simple and intuitive as possible, and there will be future endeavors to achieve that through changes to the syntax.

### Components
Components are defined by `DEFINE <component_identifier> {}`. An example of a component is

```
DEFINE ai_scheduling
{
    SUBSYSTEM scheduler
    FIELD memory 
        DEFAULT 128
    DEPENDENCIES
    [
        ai_tools
    ]
}
```

Here, the component `ai_scheduling` is defined, wherein it defined its subsystem as `scheduler`, and defined a configurable integer called `memory`. Lastly, it depends on the component `ai_tools`.

Within a block, such as that of a component, you can also define a set of statements, which can define fields, dependencies, relationships, variables, conditionals, and more.

### List of Statements
#### FIELD
The `FIELD` statement is used to define a configurable value associated with a component. Any element that a user can configure would have to be defined through a `FIELD` statement. 

```FIELD <identifier> DEFAULT <default_value>```

All `FIELD` statements are required by the compiler to include a `DEFAULT` as well, which ensures an easier end user experience.

#### Lists
Lists are sets of identifiers or literals that are encased in square brackets. For example:
```
DEPENDENCIES 
[
    foo
    bar
    baz
]
```
This states that a component depends on `foo`, `bar`, and `baz`. At the current moment, there are no need for seperation tokens, like commas, to exist because all current contents of a list are consistently single-token.
#### DEPENDENCIES
A component can declare its dependencies using the ```DEPENDENCIES []``` statement. It must always be followed by a list full of identifiers, which declares what other components it depends on. They also must be defined somewhere else in the source code. Whether or not it has been defined at that point is irrelevant however, because of how components are registered. An example of this statement was shown above.