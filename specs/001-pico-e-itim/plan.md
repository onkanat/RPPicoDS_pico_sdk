# Implementation Plan: Pico Eğitim Kartı – Donanım Kütüphaneleri ve Doxygen Dokümantasyonu

**Branch**: `001-pico-e-itim` | **Date**: 2025-09-09 | **Spec**: /Users/hakankilicaslan/pico/RPPicoDS_pico_sdk/specs/001-pico-e-itim/spec.md
**Input**: Feature specification from `/specs/001-pico-e-itim/spec.md`

## Execution Flow (/plan command scope)
```
1. Load feature spec from Input path
   → If not found: ERROR "No feature spec at {path}"
2. Fill Technical Context (scan for NEEDS CLARIFICATION)
   → Detect Project Type from context (web=frontend+backend, mobile=app+api)
   → Set Structure Decision based on project type
3. Evaluate Constitution Check section below
   → If violations exist: Document in Complexity Tracking
   → If no justification possible: ERROR "Simplify approach first"
   → Update Progress Tracking: Initial Constitution Check
4. Execute Phase 0 → research.md
   → If NEEDS CLARIFICATION remain: ERROR "Resolve unknowns"
5. Execute Phase 1 → contracts, data-model.md, quickstart.md, agent-specific template file (e.g., `CLAUDE.md` for Claude Code, `.github/copilot-instructions.md` for GitHub Copilot, or `GEMINI.md` for Gemini CLI).
6. Re-evaluate Constitution Check section
   → If new violations: Refactor design, return to Phase 1
   → Update Progress Tracking: Post-Design Constitution Check
7. Plan Phase 2 → Describe task generation approach (DO NOT create tasks.md)
8. STOP - Ready for /tasks command
```

**IMPORTANT**: The /plan command STOPS at step 7. Phases 2-4 are executed by other commands:
- Phase 2: /tasks command creates tasks.md
- Phase 3-4: Implementation execution (manual or via tools)

## Summary
Bu özellik, Pico-SDK tabanlı C projesindeki tüm donanım modüllerini (buttons, buzzer, keypad, lcd_i2c, sensors, stepper, led_control, init_functions) eğitim amaçlı, Doxygen ile kapsamlı biçimde belgelendirir ve üretim adımlarını standartlaştırır. Teknik yaklaşım: mevcut modüler C kodunu korumak, tüm public API’lerde Doxygen kapsamasını tamamlamak, Doxygen+Graphviz ile ./docs altında HTML/LaTeX çıktı üretmek, uyarıları minimize etmek ve ek olarak iki küçük iyileştirmeyi planlamak: (1) Buton IRQ’larında debounce/çoklu tetikleme iyileştirmesi (ISR’de gating ve minimum aralık), (2) Dökümantasyon içinde kısa “Nasıl Kullanılır” sayfaları.

## Technical Context
**Language/Version**: C (C17 uyumlu derleyici ile)  
**Primary Dependencies**: Raspberry Pi Pico SDK, Doxygen, Graphviz (dot)  
**Storage**: N/A (gömülü)  
**Testing**: Manuel donanım testi; otomatik test altyapısı yok [NEEDS CLARIFICATION]  
**Target Platform**: RP2040 (Raspberry Pi Pico), macOS üzerinde geliştirme  
**Project Type**: Single project (CMake tabanlı gömülü uygulama)  
**Performance Goals**: ISR’lerde minimum gecikme; ana döngüde stabil  
**Constraints**: Donanım zamanlaması, ISR güvenliği, sınırlı kaynaklar  
**Scale/Scope**: Eğitim kartı kapsamındaki bileşenler; tek ikili çıktı

## Constitution Check
*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

**Simplicity**:

- Projects: 1 (tek CMake hedefi)
- SDK doğrudan kullanılıyor; ek soyutlama yok
- Veri modeli basit (modül ve API yüzeyleri)
- Gereksiz kalıplar yok

**Architecture**:

- Uygulama tek ikili; modüller C dosyaları olarak ayrık
- CLI gerekmez (gömülü)
- Dökümantasyon Doxygen ile sağlanır

**Testing (NON-NEGOTIABLE)**:

- Otomatik testler yok; manuel doğrulama yapılacak [NEEDS CLARIFICATION]
- Dökümantasyon üretimi ve derleme kalite kapısı olarak kullanılacak

**Observability**:

- Seri çıktı ve LED’lerle gözlem
- Hata bağlamı basit loglarla verilir

**Versioning**:

- Sürümleme kapsam dışı; eğitim deposu

## Project Structure

### Documentation (this feature)

```text
specs/[###-feature]/
├── plan.md              # This file (/plan command output)
├── research.md          # Phase 0 output (/plan command)
├── data-model.md        # Phase 1 output (/plan command)
├── quickstart.md        # Phase 1 output (/plan command)
├── contracts/           # Phase 1 output (/plan command)
└── tasks.md             # Phase 2 output (/tasks command - NOT created by /plan)
```

### Source Code (repository root)

```text
# Option 1: Single project (DEFAULT)
src/
├── models/
├── services/
├── cli/
└── lib/

tests/
├── contract/
├── integration/
└── unit/

# Option 2: Web application (when "frontend" + "backend" detected)
backend/
├── src/
│   ├── models/
│   ├── services/
│   └── api/
└── tests/

frontend/
├── src/
│   ├── components/
│   ├── pages/
│   └── services/
└── tests/

# Option 3: Mobile + API (when "iOS/Android" detected)
api/
└── [same as backend above]

ios/ or android/
└── [platform-specific structure]
```

**Structure Decision**: Option 1 (Single project). Mevcut CMake yapısı korunur; modül dosyaları mevcut konumlarında.

## Phase 0: Outline & Research

1. **Extract unknowns from Technical Context** above:
   - For each NEEDS CLARIFICATION → research task
   - For each dependency → best practices task
   - For each integration → patterns task

2. **Generate and dispatch research agents**:

   ```text
   For each unknown in Technical Context:
     Task: "Research {unknown} for {feature context}"
   For each technology choice:
     Task: "Find best practices for {tech} in {domain}"
   ```

3. **Consolidate findings** in `research.md` using format:
   - Decision: [what was chosen]
   - Rationale: [why chosen]
   - Alternatives considered: [what else evaluated]

**Output**: research.md with key unknowns resolved (aşağıda ek dosyada)

## Phase 1: Design & Contracts

Prerequisites: research.md complete

1. **Extract entities from feature spec** → `data-model.md`:
   - Entity name, fields, relationships
   - Validation rules from requirements
   - State transitions if applicable

2. **Generate API contracts** from functional requirements:
   - For each user action → endpoint
   - Use standard REST/GraphQL patterns
   - Output OpenAPI/GraphQL schema to `/contracts/`

3. **Generate contract tests** from contracts:
   - One test file per endpoint
   - Assert request/response schemas
   - Tests must fail (no implementation yet)

4. **Extract test scenarios** from user stories:
   - Each story → integration test scenario
   - Quickstart test = story validation steps

5. **Update agent file incrementally** (O(1) operation):
   - Run `/scripts/update-agent-context.sh [claude|gemini|copilot]` for your AI assistant
   - If exists: Add only NEW tech from current plan
   - Preserve manual additions between markers
   - Update recent changes (keep last 3)
   - Keep under 150 lines for token efficiency
   - Output to repository root

**Output**: data-model.md, /contracts/* (API sözleşmeleri), quickstart.md. Test dosyaları kapsam dışı (gömülü donanım).

## Phase 2: Task Planning Approach

This section describes what the /tasks command will do - DO NOT execute during /plan

**Task Generation Strategy**:

- Load `/templates/tasks-template.md` as base
- Generate tasks from Phase 1 design docs (contracts, data model, quickstart)
- Each contract → contract test task [P]
- Each entity → model creation task [P]
- Each user story → integration test task
- Implementation tasks to make tests pass

**Ordering Strategy**:

- TDD order: Tests before implementation
- Dependency order: Models before services before UI
- Mark [P] for parallel execution (independent files)

Bu özellik için öncelikli görev başlıkları (planlanan, /tasks ile üretilecek):

- Doxygen kapsaması: public API’lerde eksik @param/@return tamamla
- Buton IRQ debounce/çoklu tetikleme iyileştirmesi (ISR gating + minimum interval)
- “Nasıl Kullanılır” kısa sayfaları (butonlar, LCD, buzzer, sensör, step motor) docs altında
- README’de kalite kapıları ve sorun giderme notları

**Estimated Output**: 12-20 maddelik odaklı görev listesi in tasks.md

**IMPORTANT**: This phase is executed by the /tasks command, NOT by /plan

## Phase 3+: Future Implementation

These phases are beyond the scope of the /plan command

**Phase 3**: Task execution (/tasks command creates tasks.md)
**Phase 4**: Implementation (execute tasks.md following constitutional principles)
**Phase 5**: Validation (run tests, execute quickstart.md, performance validation)

## Complexity Tracking

Fill ONLY if Constitution Check has violations that must be justified

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| [e.g., 4th project] | [current need] | [why 3 projects insufficient] |
| [e.g., Repository pattern] | [specific problem] | [why direct DB access insufficient] |


## Progress Tracking

This checklist is updated during execution flow

**Phase Status**:

- [x] Phase 0: Research complete (/plan command)
- [x] Phase 1: Design complete (/plan command)
- [ ] Phase 2: Task planning complete (/plan command - describe approach only)
- [ ] Phase 3: Tasks generated (/tasks command)
- [ ] Phase 4: Implementation complete
- [ ] Phase 5: Validation passed

**Gate Status**:

- [x] Initial Constitution Check: PASS
- [x] Post-Design Constitution Check: PASS
- [ ] All NEEDS CLARIFICATION resolved (test stratejisi beklemede)
- [ ] Complexity deviations documented


---
*Based on Constitution v2.1.1 - See `/memory/constitution.md`*
