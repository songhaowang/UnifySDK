# ZigBee Protocol Controller Design

## Goal
Add a new EPC-style ZigBee protocol controller to UnifySDK that uses a real CPC/NCP-backed ZigBee integration, publishes discovered ZigBee devices into the UCL/DotDot model, and supports an initial inbound command slice for OnOff and Level.

## Scope
- New application: `applications/zigpc`
- Reuse existing Unify components for config, datastore, attribute store, DotDot specialization, MQTT, and application monitoring
- Real ZigBee CPC/NCP integration from the first milestone
- Device discovery, endpoint/cluster interview, state publication, and limited inbound commands for OnOff and Level

## Non-goals
- Broad cluster coverage beyond OnOff and Level
- A backend-agnostic protocol abstraction for multiple radio integrations
- Reintroducing the deprecated historical ZigBee controller as-is

## Architecture
The controller follows the repository's documented protocol-controller pattern and the EPC startup model.

`main.c` initializes, in order:
1. `zigpc_config`
2. application monitoring
3. datastore
4. attribute store
5. `unify_dotdot_attribute_store`
6. `uic_mqtt_dotdot`
7. ZigBee-specific runtime fixtures

ZigBee-specific logic is split into focused components:
- `zigpc_config`: CLI/config keys, cached config access
- `zigpc_datastore`: controller-specific persisted state
- `zigpc_ncp`: CPC/NCP session management and frame exchange
- `zigpc_discovery`: network bring-up, node interview, endpoint/cluster discovery
- `zigpc_attrmgmt`: attribute-store population and state synchronization
- `zigpc_command_translator`: UCL OnOff/Level command handling toward ZigBee

This keeps Unify shared services unchanged and confines ZigBee-specific behavior to application-local components.

## Data Flow
At startup, `zigpc_ncp` establishes the CPC/NCP connection and completes controller bring-up. Discovery events flow into `zigpc_discovery`, which interviews nodes, endpoints, and clusters. `zigpc_attrmgmt` materializes that information in the attribute store so the existing DotDot/MQTT path publishes the UCL view.

After the initial interview, ZigBee reports continue to update the same attribute-store nodes. MQTT publication stays consistent because the shared DotDot path already reflects attribute-store changes.

For inbound commands, `zigpc_command_translator` subscribes to generated UCL commands for OnOff and Level. It resolves the target UNID, endpoint, and cluster from the attribute store, translates the command into ZigBee frames, sends them through `zigpc_ncp`, and updates desired/reported state according to transport result and later device reports.

## Reliability
`zigpc_ncp` is the main failure boundary. If CPC/NCP connection or ZigBee controller bring-up fails, startup fails loudly. If node interview, report handling, or a device command fails, the controller logs the failure, keeps running, and preserves enough persisted state to retry discovery or resynchronize later.

Initialization order matters: config before everything else, datastore before attribute store, and DotDot specialization before MQTT/DotDot. Shutdown should reverse the dependency order.

## Testing
The first milestone should add unit coverage for:
- config parsing and cached config access
- attribute-store population helpers
- UNID/endpoint/cluster resolution
- OnOff and Level command translation

Transport details should stay behind testable interfaces so discovery and translation logic can be exercised without live hardware. Real CPC/NCP behavior remains an integration concern outside these unit tests.

## Proposed Repository Shape
```text
applications/zigpc/
|-- CMakeLists.txt
|-- main.c
`-- components/
    |-- CMakeLists.txt
    |-- zigpc_config/
    |-- zigpc_datastore/
    |-- zigpc_ncp/
    |-- zigpc_discovery/
    |-- zigpc_attrmgmt/
    `-- zigpc_command_translator/
```

## Acceptance for the First Milestone
- `zigpc` builds as a new Unify application
- it starts with config, datastore, attribute store, and MQTT/DotDot wiring in place
- it connects to a real ZigBee CPC/NCP backend
- it discovers devices and publishes a UCL/DotDot representation
- it accepts OnOff and Level commands and translates them into ZigBee traffic
