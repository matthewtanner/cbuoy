# Contributing

Welcome! We love receiving contributions from our community, so thanks for stopping by! There are many ways to contribute, including submitting bug reports, improving documentation, submitting feature requests, reviewing new submissions, or contributing code that can be incorporated into the project.

This document describes our development process. Following these guidelines shows that you respect the time and effort of the developers managing this project. In return, you will be shown respect in addressing your issue, reviewing your changes, and incorporating your contributions.

**Table of Contents:**

1. [Code of Conduct](#code-of-conduct)
2. [Important Resources](#important-resources)
2. [Questions](#questions)
3. [Feature Requests](#feature-requests)
4. [Improving Documentation](#improving-documentation)
5. [Reporting Bugs](#reporting-bugs)
6. [Contributing Code](#contributing-code)
	1. [Getting Started](#getting-started)
	1. [Finding an Issue!](#finding-an-issue)
	1. [Development Process](#development-process)
	1. [Building the Project](#building-the-project)
	1. [Testing](#testing)
	1. [Style Guidelines](#style-guidelines)
	1. [Code Formatting Rules](#code-formatting)
	1. [Whitespace Cleanup](#whitespace-cleanup)
1. [Pull Request Guidelines](#pull-request-guidelines)
	1. [Review Process](#review-process)
	1. [Addressing Feedback](#addressing-feedback)
1. [Community](#community)


## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct][0]. We expect all contributors to follow the [Code of Conduct][0] and to treat fellow humans with respect.

## Important Resources

The NDBC Web Data Guide:
https://www.ndbc.noaa.gov/docs/ndbc_web_data_guide.pdf

Embedded C Coding Standard:
https://barrgroup.com/Embedded-Systems/Books/Embedded-C-Coding-Standard

## Questions

You can email me directly with questions.

## Feature Requests


Please create a new GitHub issue for any major changes and enhancements that you wish to make. Please provide the feature you would like to see, why you need it, and how it will work. Discuss your ideas transparently and get community feedback before proceeding.

Major Changes that you wish to contribute to the project should be discussed first in an GitHub issue that clearly outlines the changes and benefits of the feature.

Small Changes can directly be crafted and submitted to the GitHub Repository as a Pull Request. See the section about Pull Request Submission Guidelines, and for detailed information the core development documentation.

## Reporting Bugs

**If you find a security vulnerability, do NOT open an issue. Email tanner.matthew@gmail.com instead.**

Before you submit your issue, please [search the issue archive][6] - maybe your question or issue has already been identified or addressed.

If you find a bug in the source code, you can help us by [submitting an issue to our GitHub issue tracker][6]. Even better, you can submit a Pull Request with a fix.

## Improving Documentation

Should you have a suggestion for the documentation, you can open an issue and outline the problem or improvement you have - however, creating the doc fix yourself is much better!

If you want to help improve the docs, it's a good idea to let others know what you're working on to minimize duplication of effort. Create a new issue (or comment on a related existing one) to let others know what you're working on. If you're making a small change (typo, phrasing) don't worry about filing an issue first.

For large fixes, please build and test the documentation before submitting the PR to be sure you haven't accidentally introduced any layout or formatting issues.


## Contributing Code

Unsure where to begin contributing to Atom? You can start by looking through these beginner and help-wanted issues: Beginner issues - issues which should only require a few lines of code, and a test or two. Help wanted issues - issues which should be a bit more involved than beginner issues.

* [How to Contribute to an Open Source Project on GitHub][2]
* [Make a Pull Request][3]
* [First Timers Only][4]

### Getting Started

Install these dependencies:

libcurl
https://curl.haxx.se/libcurl/

In git it is best to isolate each topic or feature into a “topic branch”. While individual commits allow you control over how small individual changes are made to the code, branches are a great way to group a set of commits all related to one feature together, or to isolate different efforts when you might be working on multiple topics at the same time.

While it takes some experience to get the right feel about how to break up commits, a topic branch should be limited in scope to a single issue

```
# Checkout the master branch - you want your new branch to come from master
git checkout master

# Create a new branch named newfeature (give your branch its own simple informative name)
git branch newfeature

# Switch to your new branch
git checkout newfeature
```

For more information on the GitHub fork and pull-request processes, [please see this helpful guide][5].

### Finding an Issue

The list of outstanding feature requests and bugs can be found on our on our [GitHub issue tracker][6]. Pick an unassigned issue that you think you can accomplish and add a comment that you are attempting to do it.

### Development Process

This project follows the [git flow](http://nvie.com/posts/a-successful-git-branching-model/) branching model of product development.

You should be using the master branch for the most stable release; please review [release notes](https://github.com/openopps/openopps-platform/releases) regularly. We do releases every week or two and send out notes. If you want to keep up with the latest changes, we work in the `dev` branch.  If you are using dev, keep an eagle-eye on commits and/or join our daily standup.

### Building the Project

Execute 'make' to build the project

### Testing

If you add code you need to add tests! We’ve learned the hard way that code without tests is undependable. If your pull request reduces our test coverage because it lacks tests then it will be rejected.

Tests should be added to test-ndbcc.c, either directly by calling a function from another file.

### Style Guidelines

ndbc-c follows the Embedded C Coding Standard   
https://barrgroup.com/Embedded-Systems/Books/Embedded-C-Coding-Standard

### Code Formatting

The project uses uncrustify to enforce code formatting called out in the Embedded C Coding Standard.   
http://uncrustify.sourceforge.net/

Uncrustify should be run on code before submitting a pull request. This can be done be running 'make format'. 

### Whitespace Cleanup

Please submit whitespace cleanups in a separate pull request.

### Git Commit Guidelines

The first line of the commit log must be treated as as an email
subject line.  It must be strictly no greater than 50 characters long.
The subject must stand on its own and not only make external
references such as to relevant bug numbers.

The second line must be blank.

The third line begins the body of the commit message (one or more
paragraphs) describing the details of the commit.  Paragraphs are each
separated by a blank line.  Paragraphs must be word wrapped to be no
longer than 76 characters.

The last part of the commit log should contain all "external
references", such as which issues were fixed.

For further notes about git commit messages, [please read this blog post][7].

## Pull Request Process

When you are ready to generate a pull request, either for preliminary review, or for consideration of merging into the project you must first push your local topic branch back up to GitHub:

```
git push origin newfeature
```

### Review Process


> I will look at pull Requests on a regular basis.


### Addressing Feedback

Once a PR has been submitted, your changes will be reviewed and constructive feedback may be provided. Feedback isn't meant as an attack, but to help make sure the highest-quality code makes it into our project. Changes will be approved once required feedback has been addressed.

If a maintainer asks you to "rebase" your PR, they're saying that a lot of code has changed, and that you need to update your fork so it's easier to merge.

To update your forked repository, follow these steps:

```
# Fetch upstream master and merge with your repo's master branch
git fetch upstream
git checkout master
git merge upstream/master

# If there were any new commits, rebase your development branch
git checkout newfeature
git rebase master
```

If too much code has changed for git to automatically apply your branches changes to the new master, you will need to manually resolve the merge conflicts yourself.

Once your new branch has no conflicts and works correctly, you can override your old branch using this command:

```
git push -f
```

Note that this will overwrite the old branch on the server, so make sure you are happy with your changes first!

## Community


[0]: docs/CODE_OF_CONDUCT.md
[1]: style_guidelines.md
[2]: https://egghead.io/series/how-to-contribute-to-an-open-source-project-on-github
[3]: http://makeapullrequest.com/
[4]: http://www.firsttimersonly.com
[5]: https://gist.github.com/Chaser324/ce0505fbed06b947d962
[7]: http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html
