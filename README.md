
# MuOnline Launcher

MuOnline Launcher é um projeto open-source desenvolvido pela comunidade [PerfectZone](https://perfectzone.com.br). 
Este projeto tem como objetivo criar um launcher funcional para o jogo MuOnline, utilizando boas práticas de programação, Clean Code e C++ moderno.

## Requisitos

- [Git](https://git-scm.com/)
- [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)
- [C++ SDK](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-160)

## Como Contribuir

1. **Clone o Repositório:**

   ```sh
   git clone https://github.com/eumarcodev/muonline-laucher.git
   cd muonline-laucher
   ```

2. **Crie uma Nova Branch:**

   Para cada nova funcionalidade ou correção de bug, crie uma nova branch a partir de `develop`:

   ```sh
   git checkout develop
   git pull origin develop
   git checkout -b feature/nome-da-feature
   ```

3. **Faça Commit das Suas Alterações:**

   Realize commits frequentes com mensagens descritivas:

   ```sh
   git add .
   git commit -m "Descrição clara do commit"
   ```

4. **Envie Suas Alterações para o Repositório Remoto:**

   ```sh
   git push origin feature/nome-da-feature
   ```

5. **Crie um Pull Request:**

   Crie um pull request para a branch `develop` no GitHub e solicite uma revisão de código.

## Regras de Contribuição

- As alterações só serão aceitas através de pull requests.
- Siga a nomenclatura das branches:
  - `feature/nome-da-feature` para novas funcionalidades.
  - `bugfix/nome-do-bugfix` para correções de bugs.
  - `hotfix/nome-do-hotfix` para correções críticas.
  - `release/nome-da-release` para preparar uma nova versão do projeto.
- Adicione seus créditos no arquivo `README.md` na seção de Colaboradores.
- Adicione todas as alterações, correções e novas funcionalidades no arquivo `CHANGELOG.md`.

## Colaboradores

- [Marco Quadros (eumarcodev)](https://github.com/eumarcodev) - [Usuário no Fórum - marcoquadros](https://perfectzone.com.br/member.php?u=3583)
- [JanoDesign](https://www.behance.net/janodesign/projects) - [Usuário no Fórum - JanoDesign](https://perfectzone.com.br/member.php?u=3697)

## Registro de Alterações (Changelog)

Crie um arquivo `CHANGELOG.md` se não existir para registrar todas as alterações feitas no projeto. 

Utilize o formato abaixo:

```markdown
# Changelog

## [Unreleased]
### Added
- Descrição da nova funcionalidade adicionada.

### Changed
- Descrição da funcionalidade alterada.

### Fixed
- Descrição do bug corrigido.

## [1.0.0] - 2024-04-17
### Added
- Lançamento inicial do projeto MuOnline Launcher.
```

## Referências para Estudo

- [GitFlow Workflow](https://www.atlassian.com/git/tutorials/comparing-workflows/gitflow-workflow)
- [Documentação Oficial do Git](https://git-scm.com/doc)
- [GitHub Guides](https://guides.github.com/)

**Observação:** Só serão aceitas alterações seguindo as orientações acima para mantermos o projeto organizado.
