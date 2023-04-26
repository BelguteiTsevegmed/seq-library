# seq-library

#### typedef struct seq seq_t;
This is the name of a structured type that represents a collection of sequences(later seqs) with an equivalence relationship.

#### seq_t * seq_new(void);

The seq_new function creates a new empty set of seqs.

Function result:

- a pointer to a structure representing a set of seqs 
- or NULL if there was a memory allocation error; the function then sets errno to ENOMEM.

#### void seq_delete(seq_t *p);

The seq_delete function deletes a set of seqs and frees all the memory it uses. It does nothing if called with a NULL pointer. After this function executes, the pointer passed to it becomes invalid.

Function Parameter:

- p – pointer to a structure representing a set of seqs.

#### int seq_add(seq_t *p, char const *s);

The seq_add function adds the given string and all non-empty substrings prefixed to the set of seq.

Function Parameters:

- p – pointer to a structure representing a set of seqs;
- s – pointer to a string representing a non-empty string.

Function result:

- 1 – if at least one new seq has been added to the set;
- 0 - if the set of seqs has not changed;
- -1 – if any of the parameters is incorrect or a memory allocation error occurred; the function then sets errno to EINVAL or ENOMEM, respectively.

#### int seq_remove(seq_t *p, char const *s);

The seq_remove function removes the given seq and all seqs prefixed by it from the set of seqs.

Function Parameters:

- p – pointer to a structure representing a set of seqs;
- s – pointer to a string representing a non-empty string.

Function result:

- 1 – if at least one seq has been removed from the set;
- 0 - if the set of seqs has not changed;
- -1 – if any of the parameters is incorrect; the function then sets errno to EINVAL.

#### int seq_valid(seq_t *p, char const *s);

The seq_valid function checks whether the given seq belongs to the set of seqs.

Function Parameters:

- p – pointer to a structure representing a set of seqs;
- s – pointer to a string representing a non-empty seq.

Function result:

- 1 - if the seq belongs to the set of seqs;
- 0 - if the seq does not belong to the set of seqs;
- -1 – if any of the parameters is incorrect; the function then sets errno to EINVAL.

#### int seq_set_name(seq_t *p, char const *s, char const *n);

The seq_set_name function sets or changes the name of the abstraction class to which the given seq belongs.

Function Parameters:

- p – pointer to a structure representing a set of strings;
- s – pointer to a string representing a non-empty seq;
- n – pointer to a string with a new non-empty name.

Function result:

- 1 – if the name of the abstraction class has been assigned or changed;
- 0 - if the seq does not belong to the set of seqs or the abstract class name has not been changed;
- -1 – if any of the parameters is incorrect or a memory allocation error occurred; the function then sets errno to EINVAL or ENOMEM, respectively.

#### char const * seq_get_name(seq_t *p, char const *s);

The seq_get_name function returns a pointer to a string containing the name of the abstraction class to which the given seq belongs.

Function Parameters:

- p – pointer to a structure representing a set of seqs;
- s – pointer to a string representing a non-empty string.

Function result:

- a pointer to a string containing the name 
- NULL if the seq does not belong to the set of seqs or the abstraction class containing the seq has no name assigned; the function then sets errno to 0.
- NULL if any of the parameters is incorrect; the function then sets errno to EINVAL.

#### int seq_equiv(seq_t *p, char const *s1, char const *s2);

The seq_equiv function combines into a single abstraction class the abstraction classes represented by the given seqs. If both abstract classes are unnamed, the new abstract class is unnamed as well. If exactly one of the abstract classes has a name assigned to it, the new abstract class gets that name. If both abstraction classes have different names, the name of the new abstraction class is created by combining these names. If both abstract classes are assigned the same name, that name remains the name of the new abstract class.

Function Parameters:

- p – pointer to a structure representing a set of seqs;
- s1 – pointer to a string representing a non-empty seq;
- s2 – pointer to a string representing a non-empty seq.

Function result:

- 1 - if a new abstraction class was created;
- 0 – if no new abstraction class has been created, because the given seqs already belong to the same abstraction class or one of them does not belong to the set of seqs;
- -1 – if any of the parameters is incorrect or a memory allocation error occurred; the function then sets errno to EINVAL or ENOMEM, respectively.
